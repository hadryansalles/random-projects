---------------------------------------------------------------
-- file: processor.vhd
-- authors: Ricky Habegger and Hadryan Salles
-- date: 09/14/2020
-- brief: processor 
---------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity processor is
    port(
        clk, rst : in std_logic;
        p_state : out unsigned(1 downto 0); 
        p_pc : out unsigned(11 downto 0);
        p_instr : out unsigned(15 downto 0);
        p_reg0_out : out unsigned(15 downto 0);
        p_reg1_out : out unsigned(15 downto 0);
        p_alu_out : out unsigned(15 downto 0)
    );
end entity;

architecture a_processor of processor is
    -- ram interface
    component ram is
        port (
            clk      : in std_logic;
            address  : in unsigned(6 downto 0);
            wr_en    : in std_logic;
            data_in  : in unsigned(15 downto 0);
            data_out : out unsigned(15 downto 0)
        );
    end component;

    component rom is
        port (
            clk : in std_logic;
            address : in unsigned(6 downto 0);
            data_out : out unsigned(15 downto 0)
        ); 
    end component;

    component pc
        port (
            clk, rst, wr_en : in std_logic;
            data_in  :  in unsigned(11 downto 0);
            data_out : out unsigned(11 downto 0)
        );
    end component;

    component alu is
        port (
            in0, in1 : in unsigned(15 downto 0);
            sel      : in unsigned(1 downto 0);
            CY       : out std_logic; -- '1' if a borrow occurs from MSB; otherwise, '0'.
            OV       : out std_logic; -- '1' if overflow occurs; otherwise, '0'.
            S        : out std_logic; -- '1' if the operation result is negative; otherwise, '0'.
            Z        : out std_logic; -- '1' if the operation result is '0'; otherwise, '0'
            data_out : out unsigned(15 downto 0); -- result
            higher_data_out : out unsigned(15 downto 0) -- higher 16bits of result
        );
    end component;

    component state_machine is
        port (
            clk   :  in std_logic := '0';
            rst   :  in std_logic := '0';
            state : out unsigned(1 downto 0) := "00"
        );
    end component;

    component registers is
        port (
            clk : in std_logic := '0';
            rst : in std_logic := '0';
            wr_en : in std_logic := '0';
            read_reg0 : in unsigned(2 downto 0) := "000";
            read_reg1 : in unsigned(2 downto 0) := "000";
            write_reg : in unsigned(2 downto 0) := "000";
            data_in : in unsigned(15 downto 0) := "0000000000000000";
            read_data0 : out unsigned(15 downto 0) := "0000000000000000";
            read_data1 : out unsigned(15 downto 0) := "0000000000000000"
        );
    end component;

    component cu is
        port (
            state      :  in unsigned(1 downto 0) := "00";
            instr      :  in unsigned(15 downto 0) := "0000000000000000";
            nop_op     : out std_logic := '0';
            jr_op      : out std_logic := '0';
            jmp_op     : out std_logic := '0';
            add_op     : out std_logic := '0';
            sub_op     : out std_logic := '0';
            mul_op     : out std_logic := '0';
            cmp_op     : out std_logic := '0';
            be_op      : out std_logic := '0';
            blt_op     : out std_logic := '0';
            mov_op     : out std_logic := '0';
            ld_op      : out std_logic := '0';
            st_op      : out std_logic := '0';
            const_load : out std_logic := '0';
            pc_wr_en   : out std_logic := '0';
            excpt      : out std_logic := '0';
            registers_wr_en : out std_logic := '0';
            flags_wr_en     : out std_logic := '0'
        );
    end component;

    component reg16bits 
        port (
            clk, rst, wr_en : in std_logic := '0';
            data_in  : in unsigned(15 downto 0) := "0000000000000000";
            data_out : out unsigned(15 downto 0) := "0000000000000000"
        );
    end component;

    -- signals
    signal ram_in, ram_out : unsigned(15 downto 0) := "0000000000000000";
    signal ram_wr_en : std_logic := '0';
    signal ram_addr  : unsigned(6 downto 0) := "0000000";
    signal pc_in, pc_out : unsigned(11 downto 0) := "000000000000";
    signal state         : unsigned( 1 downto 0) := "00";
    signal instr         : unsigned(15 downto 0) := "0000000000000000";
    signal instr_reg_out : unsigned(15 downto 0) := "0000000000000000";
    signal nop_op     : std_logic := '0';
    signal jr_op      : std_logic := '0';
    signal jmp_op     : std_logic := '0';
    signal add_op     : std_logic := '0';
    signal sub_op     : std_logic := '0';
    signal mul_op     : std_logic := '0';
    signal cmp_op     : std_logic := '0';
    signal be_op      : std_logic := '0';
    signal blt_op     : std_logic := '0';
    signal mov_op     : std_logic := '0';
    signal ld_op     : std_logic := '0';
    signal st_op     : std_logic := '0';
    signal const_load : std_logic := '0';
    signal pc_wr_en   : std_logic := '0';
    signal excpt      : std_logic := '0';
    signal registers_wr_en : std_logic := '0';
    signal flags_wr_en : std_logic := '0';

    signal alu_in0, alu_in1 : unsigned(15 downto 0) := "0000000000000000"; 
    signal alu_sel          : unsigned(1 downto 0) := "00";
    signal alu_CY           : std_logic := '0'; 
    signal alu_OV           : std_logic := '0'; 
    signal alu_S            : std_logic := '0'; 
    signal alu_Z            : std_logic := '0'; 
    signal alu_out          : unsigned(15 downto 0) := "0000000000000000";
    signal alu_higher_out   : unsigned(15 downto 0) := "0000000000000000";

    signal flags_in    : unsigned(15 downto 0) := "0000000000000000";
    signal flags_out   : unsigned(15 downto 0) := "0000000000000000";
    signal flag_CY, flag_OV, flag_S, flag_Z : std_logic := '0';

    signal read_reg0, read_reg1, write_reg : unsigned(2 downto 0) := "000";
    signal data_reg0, data_reg1, registers_data_in : unsigned(15 downto 0) := "0000000000000000";
    
    -- Ler a memória de programa
    signal fetch_state : std_logic := '0';
    -- Ler o banco de registradores e gerar os sinais de controle
    signal decode_state : std_logic := '0';
    -- Fazer conta na ULA
    signal execute_state : std_logic := '0';

    -- registers for instruction
    signal reg1, reg2, reg3 : unsigned(2 downto 0) := "000";
    signal disp6 : unsigned(5 downto 0) := "000000";

begin
    ram_c: ram port map (
        clk => clk,
        address => ram_addr,
        wr_en => ram_wr_en,
        data_in => ram_in,
        data_out => ram_out
    );
    pc_c: pc port map (
       clk => clk,
       rst => rst,
       wr_en => pc_wr_en,
       data_in => pc_in,
       data_out => pc_out
    );
    cu_c: cu port map(
        state => state,
        instr => instr_reg_out,
        nop_op => nop_op,
        jr_op  => jr_op,
        jmp_op => jmp_op,
        add_op => add_op,
        sub_op => sub_op,
        cmp_op => cmp_op,
        be_op  => be_op,
        blt_op => blt_op,
        mul_op => mul_op,
        mov_op => mov_op,
        ld_op  => ld_op,
        st_op  => st_op,
        const_load => const_load,
        pc_wr_en => pc_wr_en,
        excpt => excpt,
        registers_wr_en => registers_wr_en
    );
    state_machine_c: state_machine port map(
        clk => clk,
        rst => rst,
        state => state
    );
    rom_c: rom port map(
        clk => clk,
        address => pc_out(6 downto 0),
        data_out => instr
    );
    instr_reg: reg16bits port map(
        clk => clk,
        rst => rst,
        wr_en => '1',
        data_in => instr,
        data_out => instr_reg_out
    );
    flags_reg: reg16bits port map(
        clk => clk,
        rst => rst,
        wr_en => flags_wr_en,
        data_in => flags_in,
        data_out => flags_out
    );
    alu_c: alu port map(
        in0 => alu_in0,
        in1 => alu_in1,
        sel => alu_sel,
        CY => alu_CY,
        OV => alu_OV,
        S => alu_S,
        Z => alu_Z,
        data_out => alu_out,
        higher_data_out => alu_higher_out
    );
    registers_c: registers port map(
        clk => clk,
        rst => rst,
        wr_en => registers_wr_en,
        read_reg0 => read_reg0,
        read_reg1 => read_reg1,
        write_reg => write_reg,
        data_in => registers_data_in,
        read_data0 => data_reg0,
        read_data1 => data_reg1
    );

    -- States
    fetch_state <= '1' when state = "00" else '0';
    decode_state <= '1' when state = "01" else '0';
    execute_state <= '1' when state = "10" else '0';

    reg1 <= instr_reg_out(2 downto 0);
    reg2 <= instr_reg_out(5 downto 3);
    reg3 <= instr_reg_out(8 downto 6);
    disp6 <= instr_reg_out(11 downto 6);

    -- Endereços dos registradores que serão lidos do banco
    read_reg0 <= reg1 when add_op = '1' or mov_op = '1' or mul_op = '1' or ld_op = '1' or st_op = '1' else
                 reg2 when sub_op = '1' or cmp_op = '1' else
                 "000";
    read_reg1 <= reg2 when add_op = '1' or mul_op = '1' or st_op = '1' else
                 reg1 when sub_op = '1' or cmp_op = '1' or jmp_op = '1' else
                 "000";

    -- Endereço do banco que será escrito
    write_reg <= reg1 when const_load = '1' else
                 reg2 when mov_op = '1' or add_op = '1' or sub_op = '1' or mul_op = '1' or ld_op = '1' else
                 "000";

    -- Dado que será escrito no banco
    registers_data_in <= data_reg0 when mov_op = '1' else
                         B"0000_000" & instr_reg_out(11 downto 3) when const_load = '1' else
                         alu_out when add_op = '1' or sub_op = '1' or mul_op = '1' else
                         ram_out when ld_op = '1' else
                         B"0000_0000_0000_0000";
   
    pc_in  <= pc_out + instr_reg_out(11 downto 0) when jr_op = '1' or 
                                    (be_op = '1' and (flag_Z = '1')) or 
                                    (blt_op = '1' and (flag_S = '1' xor flag_OV = '1')) else
                                    data_reg1(11 downto 0) + ("000" & instr_reg_out(11 downto 3))  when jmp_op = '1' else
              pc_out + 1;

    -- entradas da ula 
    alu_in0 <= data_reg0 when add_op = '1' or sub_op = '1' or cmp_op = '1' or mul_op = '1' else
               B"0000_0000_0000_0000";
    alu_in1 <= data_reg1 when add_op = '1' or sub_op = '1' or cmp_op = '1' or mul_op = '1' else
               B"0000_0000_0000_0000";
    alu_sel <= "00" when add_op = '1' else
               "01" when sub_op = '1' or cmp_op = '1' else
               "10" when mul_op = '1' else
               "11";
    
    -- entradas flags
    flags_wr_en <= '1' when (add_op = '1' or sub_op = '1' or cmp_op = '1') and execute_state = '1' else
                   '0';
    flags_in <= B"0000_0000_0000" & alu_CY & alu_OV & alu_s & alu_Z;
    flag_CY  <= flags_out(3);
    flag_OV  <= flags_out(2);
    flag_S   <= flags_out(1);
    flag_Z   <= flags_out(0);

    -- ram pins
    ram_wr_en <= '1' when st_op = '1' and execute_state = '1' else 
                 '0';
    ram_addr <= data_reg0(6 downto 0) + (disp6(5) & disp6);
    ram_in   <= data_reg1(15 downto 0);


    -- debug pins
    p_state <= state;
    p_pc <= pc_in;
    p_instr <= instr;
    p_reg0_out <= data_reg0;
    p_reg1_out <= data_reg1;
    p_alu_out <= alu_out;
end architecture;
